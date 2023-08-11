import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CreateNewSchemeGroupComponent } from './create-new-scheme-group.component';

describe('CreateNewSchemeGroupComponentComponent', () => {
  let component: CreateNewSchemeGroupComponent;
  let fixture: ComponentFixture<CreateNewSchemeGroupComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ CreateNewSchemeGroupComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(CreateNewSchemeGroupComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
