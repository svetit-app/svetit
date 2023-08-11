import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SchemeSectionComponent } from './scheme-section.component';

describe('SchemeSectionComponent', () => {
  let component: SchemeSectionComponent;
  let fixture: ComponentFixture<SchemeSectionComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ SchemeSectionComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SchemeSectionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
