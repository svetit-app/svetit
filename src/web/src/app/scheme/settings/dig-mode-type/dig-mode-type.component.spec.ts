import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DigModeTypeComponent } from './dig-mode-type.component';

describe('DigModeTypeComponent', () => {
  let component: DigModeTypeComponent;
  let fixture: ComponentFixture<DigModeTypeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DigModeTypeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DigModeTypeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
